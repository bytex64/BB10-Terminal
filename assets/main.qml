/*
 * Copyright (c) 2014 The Dominion of Awesome
 */

import bb.cascades 1.2

TabbedPane {
    Tab { //First tab
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("Tab 1") + Retranslate.onLocaleOrLanguageChanged
        Page {
            TerminalControl {
                id: terminalControl
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
                },
                ActionItem {
                    title: "Control-Z"
                    imageSource: "asset:///control-z.png"
                    onTriggered: {
                        pty.write("\x1a")
                    }
                }
            ]
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
