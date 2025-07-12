#!/bin/bash
PROJECT_DIR=$1
PLATFORM=$2
TEMPLATE_IOS=project_ios.pbxproj.template
TEMPLATE_MAC=project.pbxproj.template

if [ -z "$PROJECT_DIR" ] || [ -z "$PLATFORM" ]; then
  echo "Usage: $0 <ProjectDirName> <Platform>"
  echo "例: $0 MyApp ios"
  echo "例: $0 MyApp macos"
  exit 1
fi

PROJECT_NAME=$(basename "$PROJECT_DIR")
OUTPUT_DIR="${PROJECT_DIR}/${PROJECT_NAME}.xcodeproj"
SRC_DIR="${PROJECT_DIR}"
MAIN_FILE="${PROJECT_NAME}_main.m"

# プラットフォーム固有の設定
if [ "$PLATFORM" = "ios" ]; then
  PRODUCT_TYPE="com.apple.product-type.application"
  BUILD_SETTINGS="SDKROOT = iphoneos; TARGETED_DEVICE_FAMILY = \"1,2\"; IPHONEOS_DEPLOYMENT_TARGET = 14.0; SUPPORTS_MACCATALYST = NO;"
  PLATFORM_NAME="iphoneos"
elif [ "$PLATFORM" = "macos" ]; then
  PRODUCT_TYPE="com.apple.product-type.application"
  BUILD_SETTINGS="SDKROOT = macosx; MACOSX_DEPLOYMENT_TARGET = 10.14; SUPPORTS_MACCATALYST = NO;"
  PLATFORM_NAME="macosx"
else
  echo "エラー: サポートされていないプラットフォーム: $PLATFORM"
  echo "サポートされているプラットフォーム: ios, macos"
  exit 1
fi

# ディレクトリ作成
mkdir -p "$OUTPUT_DIR"
mkdir -p "$SRC_DIR"

# project.pbxproj生成
if [ "$PLATFORM" = "ios" ]; then
  sed "s/{{PROJECT_NAME}}/$PROJECT_NAME/g; s/{{PRODUCT_BUNDLE_IDENTIFIER}}/com.example.$PROJECT_NAME/g" "$TEMPLATE_IOS" > "$OUTPUT_DIR/project.pbxproj"
elif [ "$PLATFORM" = "macos" ]; then
  sed "s/{{PROJECT_NAME}}/$PROJECT_NAME/g; s/{{PRODUCT_BUNDLE_IDENTIFIER}}/com.example.$PROJECT_NAME/g; s|{{BUILD_SETTINGS}}|$BUILD_SETTINGS|g; s|{{PRODUCT_TYPE}}|$PRODUCT_TYPE|g; s|{{PLATFORM}}|$PLATFORM_NAME|g" "$TEMPLATE_MAC" > "$OUTPUT_DIR/project.pbxproj"
fi

# Info.plist生成
if [ "$PLATFORM" = "ios" ]; then
cat > "$SRC_DIR/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>\$(DEVELOPMENT_LANGUAGE)</string>
    <key>CFBundleExecutable</key>
    <string>\$(EXECUTABLE_NAME)</string>
    <key>CFBundleIdentifier</key>
    <string>\$(PRODUCT_BUNDLE_IDENTIFIER)</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>\$(PRODUCT_NAME)</string>
    <key>CFBundlePackageType</key>
    <string>\$(PRODUCT_BUNDLE_PACKAGE_TYPE)</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleVersion</key>
    <string>1</string>
    <key>LSRequiresIPhoneOS</key>
    <true/>
    <key>UIRequiredDeviceCapabilities</key>
    <array>
        <string>armv7</string>
    </array>
    <key>UISupportedInterfaceOrientations</key>
    <array>
        <string>UIInterfaceOrientationPortrait</string>
        <string>UIInterfaceOrientationLandscapeLeft</string>
        <string>UIInterfaceOrientationLandscapeRight</string>
    </array>
    <key>UISupportedInterfaceOrientations~ipad</key>
    <array>
        <string>UIInterfaceOrientationPortrait</string>
        <string>UIInterfaceOrientationPortraitUpsideDown</string>
        <string>UIInterfaceOrientationLandscapeLeft</string>
        <string>UIInterfaceOrientationLandscapeRight</string>
    </array>
</dict>
</plist>
EOF
elif [ "$PLATFORM" = "macos" ]; then
cat > "$SRC_DIR/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleIdentifier</key>
    <string>com.example.$PROJECT_NAME</string>
    <key>CFBundleName</key>
    <string>$PROJECT_NAME</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleExecutable</key>
    <string>\${PROJECT_NAME}</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.14</string>
    <key>NSHumanReadableCopyright</key>
    <string>Copyright © 2024 Example. All rights reserved.</string>
</dict>
</plist>
EOF
fi

# mainファイル生成
if [ "$PLATFORM" = "ios" ]; then
cat > "$SRC_DIR/$MAIN_FILE" <<EOF
#import <UIKit/UIKit.h>

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, nil);
    }
}
EOF
elif [ "$PLATFORM" = "macos" ]; then
cat > "$SRC_DIR/$MAIN_FILE" <<EOF
#import <Cocoa/Cocoa.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        return NSApplicationMain(argc, argv);
    }
}
EOF
fi