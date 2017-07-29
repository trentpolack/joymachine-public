#include "upstream/tinyexr.h"
#include <iostream>
#include <vector>
#include <msclr/marshal_windows.h>
#include <msclr/marshal.h>
#using <mscorlib.dll> // for System

using namespace System;
using namespace msclr::interop;

namespace TinyEXR {
	public ref class TinyEXRWrapper {

	public:
		int SaveEXR(array<float,3>^ rgb, String^ outputPath) {
			EXRHeader header;
			InitEXRHeader(&header);

			EXRImage image;
			InitEXRImage(&image);

			int width = rgb->GetLength(0);
			int height = rgb->GetLength(1);

			image.num_channels = 3;
			std::vector<float> images[3];
			images[0].resize(width * height);
			images[1].resize(width * height);
			images[2].resize(width * height);

			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					int i = (y * width) + (x);

					// R layer
					images[0][i] = rgb[y, x, 0];
					// G layer
					images[1][i] = rgb[y, x, 1];
					// B layer
					images[2][i] = rgb[y, x, 2];
				}
			}

			float* image_ptr[3];
			image_ptr[0] = &(images[2].at(0)); // B
			image_ptr[1] = &(images[1].at(0)); // G
			image_ptr[2] = &(images[0].at(0)); // R

			image.images = (unsigned char**)image_ptr;
			image.width = width;
			image.height = height;

			header.num_channels = 3;
			header.channels = (EXRChannelInfo*)malloc(sizeof(EXRChannelInfo) * header.num_channels);
			
			// must be (A)RGB order, since most exr viewers expect this
			strncpy(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
			strncpy(header.channels[1].name, "G", 255); header.channels[0].name[strlen("G")] = '\0';
			strncpy(header.channels[2].name, "R", 255); header.channels[0].name[strlen("R")] = '\0';

			header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
			header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
			for (int i = 0; i < header.num_channels; i++) {
				header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
				header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
			}

			const char* err;

			// Decode .NET string (for API friendliness) to const chars
			marshal_context^ context = gcnew marshal_context();
			const char* path = context->marshal_as<const char*>(outputPath); // TODO: Does this produce a leak?

			int ret = SaveEXRImageToFile(&image, &header, path, &err);
			if (ret != TINYEXR_SUCCESS) {
				fprintf(stderr, "Save EXR err: %s\n", err);
				return ret;
			}
			printf("Saved exr file. [ %s ] \n", path);

			free(header.channels);
			free(header.pixel_types);
			free(header.requested_pixel_types);
		}
	};
}