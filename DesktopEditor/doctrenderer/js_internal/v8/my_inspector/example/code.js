function main() {
    console.log('Hello, ChromeDevTools!');
    console.log("I heard you're an amazing tool! I'm here to play with you!");
    for (var i = 1; i <= 5; ++i) {
        console.log(i + " * " + i + " = " + i * i);
    }
    var a = 7;
    var b = 5;
    var c = a * b;
    console.log(a + " * " + b + " = " + c);
}

var sayHi = function (name) {
    return "Hi, " + name;
};

main();
sayHi('Foo');
