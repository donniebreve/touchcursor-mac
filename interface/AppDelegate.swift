import Cocoa
import SwiftUI

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    var window: NSWindow!
    var statusBarController: StatusBarController!
    @IBOutlet weak var menu: NSMenu!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        statusBarController = StatusBarController.init(menu!)
        createAboutWindow()
        if bindInput() < 1 {
            print("error: Failed to capture the input device.")
            print("error:     Either the device is already captured, or input monitoring")
            print("error:     has not been allowed for this executable in 'Security & Privacy'.")
            return
        }
        if bindOutput() < 1 {
            print("error: Failed to connect to the output device. Make sure Accessibility is set in settings")
            return
        }
        startRunLoop()
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    func createAboutWindow() {
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 480, height: 300),
            styleMask: [.titled, .closable, .miniaturizable, .resizable, .fullSizeContentView],
            backing: .buffered, defer: false)
        window.isReleasedWhenClosed = false
        window.center()
        window.setFrameAutosaveName("About")
        window.contentView = NSHostingView(rootView: AboutView())
    }
    
    @IBAction func about(sender: AnyObject) {
        window.orderFront(nil)
    }
}
