// Tabbed Pane project template
import bb.cascades 1.0
import "home"

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
        HomePage {
            title: qsTr("Eco Bodhi")
        }
    }
    Tab {
        title: qsTr("Interactive Map")
        MapPage {
            title: qsTr("Interactive Map")
        }
    }
    Tab {
        title: qsTr("Top Countries")
        TopCountriesPage {
            title: qsTr("Top Countries")
        }
    }
    Tab {
        title: qsTr("Eco Facts")
        EcoFactsPage {
            title: qsTr("Eco Facts")
        }
    }
    Tab {
        title: qsTr("Eco Tips")
        EcoTipsPage {
            title: qsTr("Eco Tips")
        }
    }
    onCreationCompleted: {
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
