const childProcess = require('child_process');
const electron = require('electron');
const gulp = require('gulp');

gulp.task('devstart',
  gulp.series('clean', 'build', gulp.parallel('watch', (done) => {
    childProcess.spawn(electron, ['.'], { stdio: 'inherit' })
    .on('close', () => {
      done(); // User closed app, we're done.
    });
})));

gulp.task('start', gulp.series('build', gulp.parallel('watch', (done) => {
  childProcess.spawn(electron, ['.'], { stdio: 'inherit' })
  .on('close', () => {
    done();
  });
})));
