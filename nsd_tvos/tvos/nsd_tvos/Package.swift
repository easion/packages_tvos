// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "nsd_tvos",
    platforms: [
        .tvOS("13.0"),
    ],
    products: [
        .library(name: "nsd-tvos", targets: ["nsd_tvos"]),
    ],
    targets: [
        .target(
            name: "nsd_tvos",
            path: "Sources/nsd_tvos"
        ),
    ]
)
