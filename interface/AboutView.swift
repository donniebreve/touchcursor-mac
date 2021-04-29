import SwiftUI

struct AboutView: View {
    var body: some View {
        HStack{
            Image(nsImage:NSImage(named:NSImage.Name("keyboard_128"))!).padding()
            VStack {
                Text("touchcursor").font(.title) + Text("mac")
                Text("Version 0.1")
                Text("A lightweight key mapper.")
                    .font(Font.system(size: 10))
                    .italic()
                    .frame(minWidth: 0, idealWidth: 0, maxWidth: 200, minHeight: 0, idealHeight: 0, maxHeight: 50, alignment: .center)
                    .padding()
                Button(
                    action: {
                        if let url = URL(string: "https://github.com/donniebreve/touchcursor-mac") {
                            NSWorkspace.shared.open(url)
                        }
                    },
                    label: {
                        Text("github")
                    })
            }.padding()
        }
    }
}

struct AboutView_Previews: PreviewProvider {
    static var previews: some View {
        AboutView()
    }
}
