import bb.cascades 1.0

Container {
    property alias title: titleLabel.text

    preferredWidth: 738
    preferredHeight: 77
    topPadding: 8
    background: moduleTopNoSeparator.imagePaint
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        leftPadding: 24
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        Label {
            id: titleLabel
            textStyle {
                base: cardTitle.style
            }
        }
    }
}