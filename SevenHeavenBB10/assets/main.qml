// Tabbed Pane project template
import bb.cascades 1.0

TabbedPane {
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                onTriggered: {
                    aboutSheet.open();
                }
            }
        ]
        attachedObjects: [
            AboutSheet {
                id: aboutSheet
            }
        ]
    }
    Tab {
        title: qsTr("Home")
        Page {
            id: tab1
            Container {
                // define tab content here
                Label {
                    text: qsTr("Home")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Interactive Map")
        Page {
            id: tab2
            Container {
                // define tab content here
                Label {
                    text: qsTr("Interactive Map")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Top Countries")
        Page {
            id: tab3
            Container {
                // define tab content here
                Label {
                    text: qsTr("Top Countries")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
            }
        }
    }
    Tab {
        title: qsTr("Eco Facts")
        Page {
            Container {
                Label {
                    text: qsTr("Eco Facts")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
            }
        }
    }
    onCreationCompleted: {
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
