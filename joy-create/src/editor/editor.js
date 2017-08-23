'use strict';

const rq = require('electron-require');
var cm = rq('codemirror/addon/runmode/runmode.node.js');

module.exports = function(code, lang, mode) {
    var out = "";

    var lang = lang.toLowerCase(),
        modeName = lang;

    if (!mode) mode = lang;

    rq('codemirror/mode/meta.js');

    cm.modeInfo.forEach(function(info) {
        if (info.mime == lang) {
            modeName = info.mode;
        } else if (info.name.toLowerCase() == lang) {
            modeName = info.mode;
            lang = info.mime;
        }
    });

    // TODO: We never use lang, which makes me think this is wrong?
    if (!cm.modes[modeName])
        rq('codemirror/mode/' + modeName + '/' + modeName + '.js');

    function esc(str) {
        const map = {
            '<': '&lt;',
            '/': '&#47;',
            '&': '&amp;',
            '>': '&gt;',
        };
        return str.replace(/[<&\/>]/g, ch => map[ch]);
    }

    var curStyle = null,
        accum = "";

    function flush() {
        accum = esc(accum);
        if (curStyle) {
            out += "<span class=\"" + curStyle.replace(/(^|\s+)/g, "$1cm-") + "\">" + accum + "</span>";
        } else out += accum;
    }

    cm.runMode(code, mode, function(text, style) {
        if (style != curStyle) {
            flush();
            curStyle = style;
            accum = text;
        } else {
            accum += text;
        }
    });
    flush();

    return out;
};