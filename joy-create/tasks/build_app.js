const gulp = require('gulp');
const less = require('gulp-less');
const clean = require('gulp-clean');
const plumber = require('gulp-plumber');
const jetpack = require('fs-jetpack');
const bundle = require('./bundle');
const utils = require('./utils');

const projectDir = jetpack;
const srcDir = jetpack.cwd('./src');
const destDir = jetpack.cwd('./app');

gulp.task('bundle', () => { return Promise.all([
  bundle(srcDir.path('background.js'), destDir.path('background.js')),
  bundle(srcDir.path('app.js'), destDir.path('app.js'))
])
});

gulp.task('less', () => {
  return gulp.src(srcDir.path('stylesheets/main.less'))
  .pipe(plumber())
  .pipe(less())
  .pipe(gulp.dest(destDir.path('stylesheets')));
});

gulp.task('environment', () => {
  const configFile = `config/env_${utils.getEnvName()}.json`;
  return Promise.all([
    projectDir.copy(configFile, destDir.path('env.json'), { overwrite: true }),
    projectDir.copy('node_modules/monaco-editor/min', destDir.path('vendor'), { overwrite: true })    
  ]);
});

gulp.task('build', gulp.parallel('bundle', 'less', 'environment'));