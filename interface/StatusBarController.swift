import AppKit

class StatusBarController {
    private var statusBar: NSStatusBar
    private var statusItem: NSStatusItem
    
    init(_ menu: NSMenu) {
        statusBar = NSStatusBar.init()
        statusItem = statusBar.statusItem(withLength: 28.0)
        statusItem.menu = menu
        if let statusBarButton = statusItem.button {
            let image:NSImage = NSImage(named:NSImage.Name("keyboard_32"))!
            image.isTemplate = true
            statusBarButton.image = image
        }
    }
}
