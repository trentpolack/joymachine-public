const electron = require( 'electron' )
const { ipcMain } = require( 'electron' )

const rq = require( 'electron-require' );

// Module to control application life.
const app = electron.app

// Module to create native browser window.
const BrowserWindow = electron.BrowserWindow

const path = require( 'path' )
const url = require( 'url' )

// Keep a global reference of the window object, if you don't, the window will be closed automatically when the JavaScript object is garbage collected.
let mainWindow
let webContents

function createWindow( ) {
    // Create the browser window.
    mainWindow = new BrowserWindow( {
        width: 800,
        height: 600,
        frame: false
//        ,
//        webPreferences: {
//            preload: path.join( __dirname, 'src/app/startup.js' )
//        }
    } )
    webContents = mainWindow.webContents

    // and load the index.html of the app.
    mainWindow.loadURL( url.format( {
        pathname: path.join( __dirname, 'index.html' ),
        protocol: 'file:',
        slashes: true
    } ) )

    // Open the DevTools.
    // mainWindow.webContents.openDevTools()
    // Show window when page is ready
    mainWindow.once( 'ready-to-show', ( ) => {
        mainWindow.show( );
    } )

    // Emitted when the window is closed.
    mainWindow.on( 'closed', function( ) {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null
    } )
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on( 'ready', createWindow )

// Quit when all windows are closed.
app.on( 'window-all-closed', function( ) {
    app.quit( )
} )

app.on( 'activate', function( ) {
    // On OS X it's common to re-create a window in the app when the dock icon is clicked and there are no other windows open.
    if ( mainWindow === null ) {
        createWindow( )
    }
} )

ipcMain.on( 'load-mesh', function( event, args ) {
    // Received from io.js, pass it to renderer.js
    // 	FIXME: this feels like the wrong way to do this for sure
    webContents.send( 'load-mesh', args );
} )