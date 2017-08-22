const path = require('path');
const DashboardPlugin = require('webpack-dashboard/plugin');

module.exports = {
    entry: path.join(__dirname, "src/app"),
    output: {
        filename: "./bundle.js"
    },
    module: {
        loaders: [{
                test: /\.js$/,
                exclude: /(node_modules|bower_components)/,
                loader: 'babel',
                query: {
                    presets: ['es2015']
                }
            },
            {
                test: /\.glsl$/,
                loader: "webpack-glsl"
            },
        ]
    },
    plugins: [
        // ... your other plugins
        new DashboardPlugin()
    ],
    devtool: 'source-map',
    devServer: {
        port: 7000
    }
}