'use strict';

const { shell, ipcRenderer } = require('electron');
const path = require('path')

const rq = require('electron-require');

window.addEventListener('load', () => {
    ipcRenderer.send('joyapp-init', null);
});