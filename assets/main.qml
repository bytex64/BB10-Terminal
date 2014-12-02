/*
 * Copyright (c) 2014 The Dominion of Awesome
 */

import bb.cascades 1.2

TabbedPane {
    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("Tab 1") + Retranslate.onLocaleOrLanguageChanged
        Page {
            ScrollView {
                id: terminalScroller
                scrollViewProperties.scrollMode: ScrollMode.Vertical

                Container {
                    Label {
                        text: terminalEmulator.screen
                        multiline: true
                        textStyle.fontFamily: "DejaVu Sans Mono"
                        id: terminalOutput
                    }
                }
            }
            actions: [
                ActionItem {
                    title: "Tab"
                    imageSource: "asset:///tab.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        pty.write("\x09");
                    }
                },
                ActionItem {
                    title: "Control-C"
                    imageSource: "asset:///control-c.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        pty.write("\x03")
                    }
                },
                ActionItem {
                    title: "Escape"
                    imageSource: "asset:///esc.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        pty.write("\x1b");
                    }
                },
                ActionItem {
                    title: "Control-D"
                    imageSource: "asset:///control-d.png"
                    onTriggered: {
                        pty.write("\x04")
                    }
                }
            ]
            keyListeners: [
                KeyListener {
                    onKeyPressed: {
                        if (event.key < 200) {
                            pty.write(event.unicode);
                        }
                    }
                }
            ]
            onCreationCompleted: {
                pty.dataReady.connect(postData);
            }
            function postData(val) {
                terminalEmulator.addData(val);
		// TODO: a better way to scroll to the bottom
                terminalScroller.scrollToPoint(0, 999999);
            }
        }
    }
    Tab {
        title: qsTr("Tab 2") + Retranslate.onLocaleOrLanguageChanged
        Page {
            Container {
                Label {
                    text: qsTr("Second tab") + Retranslate.onLocaleOrLanguageChanged
                }
            }
        }
    }
}
