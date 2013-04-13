import bb.cascades 1.0

Sheet {
    id: aboutSheet
    content: Page {
        titleBar: TitleBar {
            title: qsTr("About")
            acceptAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    aboutSheet.close();
                }
            }
        }
        Container {
            background: bgAbout.imagePaint
            preferredWidth: 768.0
            preferredHeight: 1168.0
            Container {
                ScrollView {
                    Container {
                        Label {
                            text: qsTr("More info about Macadamian, Machack and team Seven Heaven goes here.")
                            multiline: true
                        }
                    }
                }
            }
        }
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: bgAbout
            imageSource: "asset:///images/polarbear_ice_melting.jpg"
        }
    ]
}
