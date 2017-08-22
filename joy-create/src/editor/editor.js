const App = require('src/app.js');
const IO = require('src/io.js');

function init() {
    // TODO: split this into a new editor-control.js or something
    var editor = ace.edit("editor");
    editor.setTheme("ace/theme/twilight");
    editor.session.setMode("ace/mode/c_cpp");


    //	int main(int argc, char* argv[]) {
    //		printf("Hello, World!\n");
    //	  }
}

export default {
    init: init
}