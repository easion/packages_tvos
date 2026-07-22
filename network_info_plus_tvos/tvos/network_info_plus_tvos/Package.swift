// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "network_info_plus_tvos",
    platforms: [
        .iOS("13.0"),
    ],
    products: [
        .library(name: "network-info-plus", targets: ["network_info_plus_tvos"])
    ],
    dependencies: [
        .package(name: "FlutterFramework", path: "../FlutterFramework")
    ],
    targets: [
        .target(
            name: "network_info_plus_tvos",
            dependencies: [
                .product(name: "FlutterFramework", package: "FlutterFramework")
            ],
            resources: [
                .process("PrivacyInfo.xcprivacy"),
            ],
            cSettings: [
                .headerSearchPath("include/network_info_plus_tvos")
            ]
        )
    ]
)
