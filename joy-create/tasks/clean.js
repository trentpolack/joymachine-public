var gulp = require('gulp');
var clean = require('gulp-clean');

gulp.task('clean-stylesheets', function () {
    return gulp.src('app/stylesheets/*.css')
        .pipe(clean({force: true}));
});
 
gulp.task('default', ['clean-stylesheets'], function () {
    return gulp.src('app/tmp/index.js')
        .pipe(clean({force: true}))
        .pipe(gulp.dest('dist'));
});