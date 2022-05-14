import AppKit

class StatusBarController {
    private var appDelegate: AppDelegate
    private var statusItem: NSStatusItem
    private var statusBarButton: NSStatusBarButton
    private var keyboardMenu: NSMenu
    
    init(_ delegate: AppDelegate, _ menu: NSMenu, _ keyboardMenu: NSMenu) {
        appDelegate = delegate
        statusItem = NSStatusBar.system.statusItem(withLength: -1)
        statusBarButton = statusItem.button!
        statusBarButton.image = NSImage(named:"keyboard_32")
        statusBarButton.image?.isTemplate = true
        // Set the menu
        statusItem.menu = menu
        // Create keyboard selection items
        self.keyboardMenu = keyboardMenu
        updateKeyboardList(keyboardMenu)
    }
    
    @objc
    func updateKeyboardList(_ keyboardMenu: NSMenu) {
        // THIS WAS NOT FUN
        // TODO: There has to be a better way to do this
        var keyboardsPointer = UnsafeMutablePointer<KeyboardInformation>(nil)
        let count = getKeyboardList(&keyboardsPointer)
        for i in 0..<count {
            let keyboard = keyboardsPointer![Int(i)]
            let menuItem = NSMenuItem(
                title:keyboard.Name.takeRetainedValue() as String,
                action: #selector(selectKeyboard(_:)),
                keyEquivalent:"");
            menuItem.target = self
            menuItem.representedObject = keyboard;
            keyboardMenu.addItem(menuItem);
            selectKeyboard(menuItem);
        }
    }
    
    @objc
    func selectKeyboard(_ sender: AnyObject) {
        guard let menuItem = sender as? NSMenuItem else { return }
        guard let keyboard = menuItem.representedObject as? KeyboardInformation else { return }
        for item in keyboardMenu.items {
            item.state = NSControl.StateValue.off;
        }
        appDelegate.startCapture(keyboard.ProductID, keyboard.VendorID)
        menuItem.state = NSControl.StateValue.on;
    }
}
