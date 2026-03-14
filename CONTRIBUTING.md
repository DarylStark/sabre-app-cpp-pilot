# Contributing to Sabre

Thank you for your interest in contributing to Sabre!

Sabre is a platform-independent framework for programming microcontrollers in a consistent way. The core of this repository is intentionally kept portable and hardware-agnostic; platform-specific implementations live in separate repositories that depend on this one.

This document describes the high-level namespace layout and basic naming conventions so contributions stay consistent and easy to maintain.

## Namespace Structure

The Sabre framework is structured as follows:

| Namespace        | Description                                                            | Interface only |  Pure   | Dependencies                           | Remarks                       |
| ---------------- | ---------------------------------------------------------------------- | :------------: | :-----: | -------------------------------------- | ----------------------------- |
| `sabre::core`    | Core abstractions and lifecycle                                        |     **No**     |         | All interface only and pure namespaces | Shouldn't depend on concretes |
| `sabre::devices` | High-level device facades (GPS, LTE, sensors) – use HAL underneath     |     **No**     |         | `hal`, `net`, `time`, `model`          |                               |
| `sabre::hal`     | Pure hardware abstraction interfaces (GPIO, Serial, I2C, SPI, etc.)    |    **Yes**     |         | None                                   |                               |
| `sabre::impl`    | Platform implementations for interfaces                                |     **No**     |         | `hal`, `net`, `os`, `time`             |                               |
| `sabre::io`      | Streams, logging sinks, files if any (desktop only)                    |    **Yes**     |         | `hal`                                  |                               |
| `sabre::log`     | Log handlers                                                           |     **No**     |         | `io`                                   |                               |
| `sabre::model`   | Domain models (Position, SatelliteInfo…) – pure value types            |     **No**     | **Yes** | None                                   |                               |
| `sabre::net`     | Protocol-level interfaces (WiFi, SoftAP, MQTT, TCP/UDP)                |    **Yes**     |         | None                                   |                               |
| `sabre::os`      | "service" concept, threading, timers; maps to FreeRTOS/Std::thread     |    **Yes**     |         | None                                   |                               |
| `sabre::parsers` | Parsers (NMEA, UBX, JSON...); pure, platform-free                      |     **No**     | **Yes** | None                                   |                               |
| `sabre::time`    | WallClock, NTP, monotonic timers                                       |    **Yes**     |         | None                                   |                               |
| `sabre::utility` | General-purpose helpers, reusable patterns, predicates, timeouts, etc. |     **No**     |         | All interface only and pure namespaces | For non-domain-specific code  |

When adding new code, choose the namespace that best fits the responsibility of your type or function. Avoid introducing new top-level namespaces without discussion.

## Naming Conventions

Sabre follows C++ naming practices broadly aligned with the LLVM coding style, with a few project-specific preferences.

### General Principles

-   Prefer clear, descriptive names over short or clever ones.
-   Choose names that reflect the *role* and *lifetime* of an entity.
-   Be consistent with existing code in the same area.

### Namespaces

-   Use all-lowercase names for namespaces.
-   Project namespaces are rooted at `sabre`; sub-namespaces describe a domain (e.g. `sabre::hal`, `sabre::net`, `sabre::time`).
-   Avoid deep nesting unless it significantly clarifies structure.
-   The `impl` namespaces is meant for platform specific implementations. A platform specific imlpementation should always use a namespace reflecing the implementation. For instance, for RP2040 implementations, the namespace `sabre::impl::rp2040` should be used.

### Types (classes, structs, enums)

-   Use PascalCase for types:
    -   Examples: `IPv4Address`, `MQTTClient`, `WaitFor`, `SerialStreamBuf`.
-   Prefer nouns or noun phrases that describe what the type *is*.
-   Use `enum class` where appropriate; name enum types like other types (e.g. `LogLevel`).

### Functions and Methods

-   Use lowerCamelCase for functions and methods:
    -   Examples: `connect()`, `disconnect()`, `waitForCompletion()`, `setTimeoutMs()`.
-   Function names should usually be verbs or verb phrases.
-   For boolean-returning functions, names should read naturally as predicates (e.g. `isConnected()`, `hasTimedOut()`).

### Data Members and Variables

-   Use lowerCamelCase for local variables and function parameters:
    -   Examples: `timeoutMs`, `currentState`, `topicMap`.
-   For class data members:
    -   Private and protected members must be prefixed with an underscore (e.g. `_timeoutMs`, `_currentState`).
    -   Members should use plain `lowerCamelCase`.
-   Avoid one-letter variable names except for small loop indices.

### Constants

-   Use PascalCase for `constexpr`/`const` variables with meaningful semantic roles:
    -   Examples: `DefaultTimeoutMs`, `MaxTopicLength`.
-   For macros (which should be rare), use `ALL_CAPS_WITH_UNDERSCORES`.

### Files and Directories

-   Match file names to the main type or concept they define:
    -   `mqtt.hpp` → contains `MQTTClient` and closely related types.
    -   `ipv4_address.hpp` → contains `IPv4Address`.
-   Use lowercase with underscores for file names.
-   Place files under a directory that matches their namespace (e.g. `sabre/net`, `sabre/hal`, `sabre/utility`).

### Exceptions and Error Types

-   Exception types should be named with a `...Exception` suffix (e.g. `TimeoutException`, `ConfigException`).
-   Base classes for exceptions should be named with `...Error`.
-   Place generic, framework-wide exceptions in `sabre::core`.
-   Place highly domain-specific error types near the code that uses them (e.g. `sabre::net` for protocol-specific errors).

## Code Style

All code must be formatted according to the project’s `.clang-format` file. Key style rules:

-   **Base style:** LLVM
-   **Indentation:** 4 spaces, no tabs
-   **Column limit:** 80 characters
-   **Braces:** Allman style (braces on their own line)
-   **Short functions:** Only empty short functions may be on a single line
-   **Standard:** C++20
-   **Bracket alignment:** Align after open bracket
-   **Namespace indentation:** All namespaces are indented
-   **Access modifier offset:** Access specifiers (`public:`, `private:`) are outdented by 4 spaces
-   **Always use braces:** Braces must always be present, even for single-line blocks (`RemoveBracesLLVM: false`)

To ensure consistency, run `clang-format` before submitting code, or use an editor that formats on save.

When contributing, keep new code as generic and portable as possible. If you are unsure where a new type or function belongs, open an issue or pull request and describe your intent.
