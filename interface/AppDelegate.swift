import Cocoa
import SwiftUI

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    var aboutWindow: NSWindow!
    var statusBarController: StatusBarController!

    // Outlets
    @IBOutlet weak var menu: NSMenu!
    @IBOutlet weak var keyboardMenu: NSMenu!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        statusBarController = StatusBarController.init(menu!)
        createAboutWindow()
        readConfiguration()
        if bindInput() < 1 {
            print("error: Failed to capture the input device.")
            print("error:     Either the device is already captured, or input monitoring")
            print("error:     has not been allowed for this executable in 'Security & Privacy'.")
            return
        }
        if bindOutput() < 1 {
            print("error: Failed to connect to the output device.")
            return
        }

        startRunLoop()
    }

    func applicationWillBecomeActive(_ notification: Notification) {
        keyboardMenu.addItem(NSMenuItem(title: "Item 01", action: nil, keyEquivalent: ""))
        keyboardMenu.addItem(NSMenuItem(title: "Item 02", action: nil, keyEquivalent: ""))
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func createAboutWindow() {
        aboutWindow = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 480, height: 300),
            styleMask: [.titled, .closable, .miniaturizable, .resizable, .fullSizeContentView],
            backing: .buffered, defer: false)
        aboutWindow.isReleasedWhenClosed = false
        aboutWindow.center()
        aboutWindow.setFrameAutosaveName("About")
        aboutWindow.contentView = NSHostingView(rootView: AboutView())
    }
    
    // Actions
    @IBAction func about(sender: AnyObject) {
        aboutWindow.orderFront(nil)
    }
}
