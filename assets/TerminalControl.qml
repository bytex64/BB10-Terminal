import bb.cascades 1.2
import terminal.pty 1.0
import terminal.emulator 1.0

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
    attachedObjects: [
        LayoutUpdateHandler {
            onLayoutFrameChanged: {
                terminalView.preferredHeight = layoutFrame.height;
            }
        },
        Pty {
            id: pty
        },
        TerminalEmulator {
            id: terminalEmulator
        }
    ]
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
        pty.open();
    }
    
    function setScreen(l) {
        terminalView.postMessage(JSON.stringify({
            type: "setScreen",
            content: l
        }));
    }
    
    function write(s) {
        pty.write(s);
    } 
}
