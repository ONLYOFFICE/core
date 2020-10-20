var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');

  fs = require('fs')
  fs.readFile('C://Users//maxim//Downloads//Telegram Desktop/1025.txt', 'utf8', function (err,data) {
    fs.writeFile('C://Users//maxim//Downloads//Telegram Desktop/helloworld.txt', 'Hello World!', function (err) {
      if (err) return console.log(err);
      console.log('Hello World > helloworld.txt');
    });


    if (err) {
      return console.log(err);
    }
    console.log(fs);
  });

});

module.exports = app;


