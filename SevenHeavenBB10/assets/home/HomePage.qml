import bb.cascades 1.0
import eco.bodhi 1.0

Page {
    property alias title: homeTitleBar.title
    titleBar: TitleBar {
        id: homeTitleBar
    }
    Container {
        background: bgHome.imagePaint
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
            ImagePaintDefinition {
                id: bgHome
                imageSource: "asset:///images/sprout.jpg"
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
