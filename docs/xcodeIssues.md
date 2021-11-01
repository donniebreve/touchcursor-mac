# Issues

# The application fails to open keyboards or send output when debugging in XCode
XCode is just... problematic sometimes. Here are a few things to try:

1. Make sure the scheme for the project is set to run the application as root
2. Remove the build folder completely `~/Library/Developer/Xcode/DerivedData/touchcursor-mac-.../`
3. Remove all entries and then manually add the XCode touchcursor-mac build output to the Accessibility and Input Monitoring lists under Security and Privacy
    1. The buid output folder is typically `~/Library/Developer/Xcode/DerivedData/touchcursor-mac-.../Build/Products/Debug`