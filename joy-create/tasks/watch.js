const gulp = require('gulp');
const watch = require('gulp-watch');
const batch = require('gulp-batch');

gulp.task('watch', () => {
	const beepOnError = (done) => {
	  return (err) => {
		if (err) {
		  utils.beepSound();
		}
		done(err);
	  };
	};
  
	watch('src/**/*.js', batch((events, done) => {
	  gulp.start('bundle', beepOnError(done));
	}));
	watch('src/**/*.less', batch((events, done) => {
	  gulp.start('less', beepOnError(done));
	}));
});