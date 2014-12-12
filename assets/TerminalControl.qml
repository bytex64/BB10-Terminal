import bb.cascades 1.2

Container {
    property variant appScene: Application.scene
    WebView {
        url: "local:///assets/TerminalControl.html"
        id: terminalView
        onMessageReceived: {
            var msg = JSON.parse(message.data);
            if (msg.type == "input") {
                inputReceived(msg.content);
            } else if (msg.type == "ready") {
                terminalReady();
            }
        }
    }
    attachedObjects: LayoutUpdateHandler {
        onLayoutFrameChanged: {
            terminalView.preferredHeight = layoutFrame.height;
        }
    }
    onAppSceneChanged: {
        terminalView.requestFocus();
    }
    onCreationCompleted: {
        pty.dataReady.connect(terminalEmulator.addData);
        terminalEmulator.screenChanged.connect(setScreen);
        terminalEmulator.sizeChanged.connect(pty.setSize);
        terminalEmulator.setSize(32, 14);
        inputReceived.connect(pty.write);        
    }
    
    signal sizeChanged(int w, int h);
    signal inputReceived(string s);
    
    function terminalReady() {
        setScreen(terminalEmulator.screen);
    }
    
    function setScreen(l) {
        terminalView.postMessage(JSON.stringify({
            type: "setScreen",
            content: l
        }));
    } 
}
