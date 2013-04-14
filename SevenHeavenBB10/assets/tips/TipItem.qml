import bb.cascades 1.0

Container {
    Container {
        topPadding: 2.0
        bottomPadding: 2.0
        leftPadding: 16.0
        rightPadding: 16.0
        Container {
        Label {
            text: ListItemData.title
            textStyle {
                base: titleStyleDef.style
            }
        }}
        Container {
        Label {
            text: ListItemData.description
            textStyle {
                base: descStyleDef.style
            }
        }}
    }
    Container {
    Divider {}}
    attachedObjects: [
        TextStyleDefinition {
            id: titleStyleDef
            color: Color.Black
            fontFamily: "SlatePro-Condensed"
            fontSize: FontSize.Large
            fontWeight: FontWeight.W600
        },
        TextStyleDefinition {
            id: descStyleDef
            color: Color.Black
            fontFamily: "SlatePro-Condensed"
            fontSize: FontSize.Small
            fontWeight: FontWeight.W400
        }
    ]
}
