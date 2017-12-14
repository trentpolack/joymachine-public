import './helpers/context_menu.js';
import './helpers/external_links.js';

// All stuff below is just to show you how it works. You can delete all of it.
import { remote } from 'electron';
import jetpack from 'fs-jetpack';
import env from './env';

import program from './program/program';
import { scene } from './viewport/scene';

const app = remote.app;
const appDir = jetpack.cwd(app.getAppPath());

const manifest = appDir.read('package.json', 'json');
const osMap = {
  win32: 'Windows',
  darwin: 'macOS',
  linux: 'Linux',
};

scene.init( );