using CommandLine;
using System;
using System.Linq;

namespace Joy.Kernels
{
    class Program
    {
        private static double Distance(int x1, int y1, int x2, int y2)
        {
            double dx = (x2 - x1);
            double dy = (y2 - y1);
            return Math.Sqrt(dx*dx + dy*dy);
        }

        private static double Lerp(double t, double minValue, double maxValue)
        {
            return minValue + (t * (maxValue - minValue));
        }

        private static double Clamp(double t, double min, double max)
        {
            return Math.Max(min, Math.Min(max, t));
        }

        static float[,,] GenerateSphereCorona(double centreValue, double falloffDistance, double falloffPowerFactor, int imageWidth)
        {
            // Make a sphere
            var buf = new float[imageWidth, imageWidth, 3];

            for (var y = 0; y < imageWidth; ++y)
            {
                for (var x = 0; x < imageWidth; ++x)
                {
                    // R, G, B
                    var pixelDistance = Distance(x, y, imageWidth / 2, imageWidth / 2); // not gonna be quite centered but nobody will notice

                    var f = Math.Max(0,
                        Math.Pow(1.0 - Clamp(pixelDistance / falloffDistance, 0, 1),
                        falloffPowerFactor))
                        * centreValue;

                    buf[y, x, 0] = (float)f;
                    buf[y, x, 1] = (float)f;
                    buf[y, x, 2] = (float)f;
                }
            }

            return buf;
        }

        [Verb("sphere", HelpText="Do a sphere corona, falling off from a maximum value at the center.", Hidden=false)]
        class SphereCoronaOptions
        {
            [Option('c', "center", Required = false, HelpText = "The maximum intensity at the center of the corona.")]
            public double CenterValue { get; set; }

            [Option('r', "radius", Required = false, HelpText = "The radius, in pixels, where the intensity falls off entirely.")]
            public double FalloffDistance { get; set; }

            [Option('p', "power", Required = false, HelpText = "Controls how quickly intensity ramps towards the falloff limit.")]
            public double FalloffPower { get; set; }

            [Option('w', "width", Required = false, HelpText = "The width in pixels. This will also be the height of the resulting image, as it is square.")]
            public int Width { get; set; }

            [Option('o', "output", HelpText = "Path of the output EXR file that will contain your corona image.")]
            public string OutputPath { get; set; }

            public SphereCoronaOptions()
            {
                CenterValue = 2.0;
                FalloffPower = 2.0; // a good basic setting.
                Width = 1024; // same.
                FalloffDistance = Width * 0.5; // will need to adjust these in pairs.
                OutputPath = "kernel.exr";
            }
        }

        static void Main(string[] args)
        {
            float[,,] buf = null;
            string outputPath = null;

            var result = Parser.Default.ParseArguments<SphereCoronaOptions>(args) // TODO: add more verb types
                .WithParsed<SphereCoronaOptions>(opts =>
                {
                    // build a sphere corona ranging from the centre to its extent radius
                    buf = GenerateSphereCorona(opts.CenterValue, opts.FalloffDistance, opts.FalloffPower, opts.Width);
                    outputPath = opts.OutputPath;
                });

            if(!string.IsNullOrWhiteSpace(outputPath) && null != buf)
            {
                var w = new TinyEXR.TinyEXRWrapper();
                w.SaveEXR(buf, outputPath);
            }
        }
    }
}
