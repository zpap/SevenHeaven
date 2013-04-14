import bb.cascades 1.0
import eco.bodhi 1.0

Container {
    property string quoteBody: ""
    property string quoteAuthor: ""
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
                text: quoteBody + " -- " + quoteAuthor
                multiline: true
            }
        }
    }
    Container {
        preferredWidth: 738
        preferredHeight: 33
        background: moduleBottom.imagePaint
    }
    attachedObjects: [
        QuotesController {
            id: quotes
        }
    ]
    onCreationCompleted: {
        var quoteMap = quotes.getRandomQuote();
        quoteBody = quoteMap.quote;
        quoteAuthor = quoteMap.author;
    }
}
