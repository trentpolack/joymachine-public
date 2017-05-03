const { shell, ipcRenderer } = require('electron');
const { dialog } = require('electron').remote; // TODO: Am I doing this right if I need 'remote'?
const importMeshButton = document.querySelector("#import-mesh")
const getHelpButton = document.querySelector("#get-help")

importMeshButton.addEventListener('click', () => {
  dialog.showOpenDialog(shell.BrowserWindow, {
    title: 'Select model',
    buttonLabel: 'Import',
    properties: [ 'openFile' ],
    filters: [
      { name: "OBJ meshes", extensions: ['obj']},
      { name: "All files", extensions: ['*']}
    ]
  }, 
  (selectedPaths) => {
    if(selectedPaths.length > 0) {
      // Since we don't have 'multiPaths' turned on above,
      // we can assume for now that the user was not allowed to select
      // multiple paths...
      toLoad = selectedPaths[0]
      ipcRenderer.send('load-mesh', toLoad)
    }
  })
})