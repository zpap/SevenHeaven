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
            Label {
                text: qsTr("More info about Macadamian, Machack and team Seven Heaven goes here.")
                multiline: true
            }
        }
    }
}
