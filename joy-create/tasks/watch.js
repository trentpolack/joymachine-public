const gulp = require('gulp');
const watch = require('gulp-watch');
const batch = require('gulp-batch');

gulp.task('watch', (done) => {
	const beepOnError = (done) => {
	  return (err) => {
		if (err) {
		  utils.beepSound();
		}
		done(err);
	  };
	};

	watch('src/**/*.js', batch((events, done) => {
    console.log('bundling js');
	  gulp.start('bundle', beepOnError(done));
	}));
	watch('src/**/*.less', batch((events, done) => {
    console.log('rebuilding less');
	  gulp.start('less', beepOnError(done));
	}));

  done();
});
