const childProcess = require('child_process');
const electron = require('electron');
const gulp = require('gulp');

gulp.task('devstart', 
  gulp.series('clean', 'build', gulp.parallel('watch', () => {
    childProcess.spawn(electron, ['.'], { stdio: 'inherit' })
    .on('close', () => {
      // User closed the app. Kill the host process.
      process.exit();
    });
})));

gulp.task('start', gulp.series('build', gulp.parallel('watch', () => {
  childProcess.spawn(electron, ['.'], { stdio: 'inherit' })
  .on('close', () => {
    // User closed the app. Kill the host process.
    process.exit();
  });
})));