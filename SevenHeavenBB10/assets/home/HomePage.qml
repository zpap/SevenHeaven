import bb.cascades 1.0

Page {
    property alias title: homeTitleBar.title
    titleBar: TitleBar {
        id: homeTitleBar
    }
    Container {
        background: Color.create("#5574F2")
        ScrollView {
            Container {
                topPadding: 24
                rightPadding: 15
                leftPadding: 15
                HomeCardQuote {
                }
            }
        }
        attachedObjects: [
            ImagePaintDefinition {
                id: moduleMiddle
                repeatPattern: RepeatPattern.Y
                imageSource: "asset:///images/module-middle.png"
            },
            ImagePaintDefinition {
                id: moduleTopSeparator
                imageSource: "asset:///images/module-top-sep.png"
            },
            ImagePaintDefinition {
                id: moduleTopNoSeparator
                imageSource: "asset:///images/module-top-no-sep.png"
            },
            ImagePaintDefinition {
                id: moduleBottom
                imageSource: "asset:///images/module-bottom.png"
            },
            TextStyleDefinition {
                id: cardTitle
                color: Color.create("#1e82ce")
                fontFamily: "SlatePro-Condensed"
                fontSize: FontSize.Large
                fontWeight: FontWeight.W500
            }
        ]
    }
}
