# Change Log
This project adheres to Semantic Versioning

## [Unreleased]
### Added
- Add a sticker system.
- Add a buffer in the main window for typing notes that may have stickers in
  them.
- Add a notes dialog that displays your saved notes and has a place to type new
  ones.
- Add note saving feature to store notes for future use.

### Fixed
- Fix various bugs.

## [1.2.0] - 2017-02-10
### Added
- Add change log. Incomplete because it wasn't done earlier.

### Changed
- Switched to a different dictionary with more sensible words.
- Missed words are now scored based on their Levenshtein distance.
- Some UI elements have been changed to reduce clutter.

## [1.1.2] - 2017-02-08
### Added
- Add random capitalization feature. The advanced test now randomly capitalizes
  10% of words which can be customized in the custom test.

### Fixed
- Make history dialog erase information when the erase button is clicked, not
  just remove the file.
- Fix random number code.
- Remove personal files that weren't supposed to be there.

## [1.1.1] - 2017-02-07
### Fixed
- Fix crash with certain compilers.

## [1.1.0] - 2017-02-07
### Added
- Add statistics tracking feature including average, record, and standard
  deviation of the user's WPM.

### Fixed
- Fixed allocation error when building for Release with CMake.

### Changed
- Move more of the config to ini-like key files.
- Update trouble words function to a new algorithm.

## [1.0.3] - 2017-02-03
### Fixed
- General fixes, a lot of them.

### Added
- Add license headers.

### Changed
- Did large code refactor. Fixed a lot of formatting.
- Write comments.

## [1.0.2]

## [1.0.1]

## [1.0.0]
