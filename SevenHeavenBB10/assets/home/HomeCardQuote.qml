import bb.cascades 1.0

Container {
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }
    HomeCardTitle {
        title: qsTr("Quote of the day")
    }
    Container {
        background: moduleMiddle.imagePaint
        Container {
            id: quote
            topPadding: 5
            bottomPadding: 2
            leftPadding: 24
            rightPadding: 24
            preferredWidth: 738
            
            Label {
                text: qsTr("\"We will add rotating quotes here, pulled from the server\" -- Rob Woods")
                multiline: true
            }
        }
    }
    Container {
        preferredWidth: 738
        preferredHeight: 33
        background: moduleBottom.imagePaint
    }
}
