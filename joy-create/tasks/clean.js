var gulp = require('gulp');
var clean = require('gulp-clean');

gulp.task('clean-stylesheets', function () {
    return gulp.src('app/stylesheets/*.css', {read: false})
        .pipe(clean({allowEmpty: true}));
});

gulp.task('clean-vendor', function () {
    return gulp.src('app/vendor', {read: false, allowEmpty: true})
        .pipe(clean());
});

gulp.task('clean-app', gulp.parallel(
    function () {
        return gulp.src('app/*.js', {read: false})
            .pipe(clean({allowEmpty: true}));
    },
    function () {
        return gulp.src('app/*.js.map', {read: false})
            .pipe(clean({allowEmpty: true}));
    })
);

gulp.task('clean', gulp.series('clean-vendor','clean-stylesheets','clean-app'));