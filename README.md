# TwitterCloneApp

TwitterClone is a cross-platform client app that uses my [backend](https://github.com/1Kuso4ek1/TwitterClone).  
It was made using C++, Qt 6.9.1 and QML.

## Features

* **Auth:** users login into their accounts using Google OAuth2.
* **Feed:** check what other users are talking about.
* **User profile:** check any profile you encounter (including yours).
* **Customization:** pick your own display name and username, change avatar to whatever you want.
* **Create posts:** write about your day or whatever you like and publish it.
* **Delete posts:** if you don't like what you've written...
* **Security:** none of the sensitive info is shared with the client (besides access/refresh tokens, lol).

## Showcase

<img src="https://github.com/user-attachments/assets/cd9a852d-8f9a-43f5-8a75-8e1e29ada44e" width=225 height=500>
<img src="https://github.com/user-attachments/assets/3b9b5d97-4fc5-4863-a0ab-ea0f19051e7f" width=225 height=500>
<img src="https://github.com/user-attachments/assets/d3debb21-e427-46e1-8a36-ec6bec0293d9" width=225 height=500>

## Project structure

```
.
├── android/         # Android-specific
├── config/          # Config files (template and the actual config.json)
├── include/         # .hpp headers
├── libraries/       # Submodules (Android OpenSSL)
├── qml/             # .qml UI files
└── src/             # .cpp sources
```

## C++ business logic
* `Api`: Singleton class for Api-specific operations.
* * `Config`: Loads `config.json` configuration.
* * `RequestHandler`: Helper class that handles network requests and possible errors.
* * `AuthApi`: Handles authentication-related API calls.
* * * `AuthManager`: Manages the OAuth2 flow (base class).
* * * `AuthManagerNative`: Implementation that uses QOAuth2AuthorizationCodeFlow.
* * * `AuthManagerWASM`: An implementation for WebAssembly built from the ground up.
* * * * `PKCEUtils`: Utilities for PKCE (Proof Key for Code Exchange) flow - used in WASM OAuth2 impl.
* * * `TokenStorage`: Saves and loads access/refresh tokens.
* * `PostsApi`: Handles posts-related API calls.
* * `UsersApi`: Handles user-related API calls.

## QML interface logic
* `Main.qml`: Main application window.
* `Login.qml`: Login screen with Google OAuth2.
* `Feed.qml`: Displays the feed of posts.
* `Post.qml`: Represents a single post in the feed.
* `NewPost.qml`: A simple 'post' editor popup.
* `Profile.qml`: Displays user profiles with their posts.
* `Avatar.qml`: A reusable rounded avatar component.
* `Navigation.qml`: Singleton for managing navigation between screens.
* `EditProfile.qml`: Profile editing popup.
* `FormField.qml`: A reusable component for input field with header and symbol-counting footer.
* `ImageViewer.qml`: A popup that displays whatever image you want.

## Configuration

For this application to work, you'll need to create a `config/config.json` (and `config/config-wasm.json` for building WASM app, because Desktop and Web apps require different kinds of client ids) based off of the `config/config-template.json`.  
You will need to specify some URLs of your backend instance and OAuth2 client id (with the type `Desktop` or `Web Application`, depending on what you're building).

```json
{
    "base_url": "https://your-backend.com/api",
    "oauth2": {
        "token_url": "https://your-backend.com/oauth/token",
        "refresh_url": "https://your-backend.com/oauth/token",
        "client_id": "your_client_id"
    }
}
```

## Building

### Prerequisites

* **Qt 6.9.1** or higher, preferably with **Qt Creator** and every other optional module.
* A compiler with **C++20** support 
* **CMake 3.16** or higher
* **Android SDK** and **NDK** (if you want to build for Android)
* **XCode** (if you want to build for iOS (good luck with that, I don't have a Mac to test it))

For a more convenient building experience for both Desktop and Mobile build, you can use **Qt Creator**.  
Either way, you'll have to follow the steps below:

### Cloning

```bash
git clone --recursive --shallow-submodules --depth=1 https://github.com/1Kuso4ek1/TwitterCloneApp.git
cd TwitterCloneApp
mkdir build && cd build
```

### Building

Then it's as simple as with every CMake project:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

This will build the project for Desktop. And again, if you want to build for Android, you can use Qt Creator to set up the Android kit and build it from there.
