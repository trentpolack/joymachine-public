v0.0.3 / 2017-05-27
===================

  * Adding AutoHotkey script for Windows' SnippingTool.
  * Words are hard.
  * Added a brief summary on the ue4-source folder.
  * Fixed up CollisionShapeSet. Added basic blueprintable enum for basic shapes.
  * General-purpose collision shape set for computing box/sphere/capsule shapes (rather than having to choose a specific one at code-time).
  * Even more convention updates as I read an awful class.
  * Added reference and link to the conventions doc.
  * Fixed sub-lists because I'm an idiot.
  * Greatly elaborated on the standard practices and conventions.
  * Consistency fix in the powershell folder.
  * Added an old material-based procedural water and wave generation/rendering asset set.
  * Actually changing the name of the folder in the README like I should have.
  * Removed "all rights reserved" heading.
  * Updated the UE4 general README with a link to the source control README.
  * Renamed git-template folder to git, since it's not just templates anymore.
  * Added even more specifics on customizing git for optimal everyday use.
  * Wrote a monologue on actors, components, and actor lifecycle.
  * Fix default camera
  * flip code and viewport panes
  * can load teapot, next up appending to the scene
  * Experimenting more with load code. Fixed broken IPC message handlers.
  * Figuring out how to handle events and OS integration, learning Electron as I go. Probably made a ton of mistakes.
  * make render-shell update when window is resized, take up entire pane
  * import babylon, create a test scene on the right pane
  * Added use information for the environment MPC and foliage shader.
  * Actually making the changelog reflect changes made.
  * Updated changelog for v0.0.2 release.

v0.0.2 / 2017-05-02
===================

  * Added an updated foliage standard shader that reads wind values from an MPC (in game_data/env).
  * Added a README.me file to the git-template gitconfig template.
  * Added two new aliases to the gitconfig.template added: git tree -- Just prints a pretty-formatted revision history in tree form. added: git cl -- NOTE: Required git-extras installed (for now). Generated a simple CHANGELOG based on commit log.
  * Added CHANGELOG.md link in the README.
  * Added a change log.
  * Made the root README suck marginally less.
  * Revamped m_foliage_master (just so happened to be working on our foliage today. COIIIINCIDENCE).
  * moved a lot of 3rd-party files into 'vendor' and also added babylon.js
  * Adding dumb changelog, but better than nothing. Still trying to find a good generator.
  * Additional information on PBR adherence.
  * Added information on the new standard shader library.
  * Adding our standard material library.
  * Removing the docs subfolder.
  * added the ACE js editor
  * add fancy icons to the buttons
  * touchups
  * quit on last window closed, even on macosx
  * master-detail demo layout
  * add css, fonts from photon UI framework
  * readme update
  * revised readme
  * electron-quick-start shell added

v0.0.1 / 2017-04-30
===================

  * Tweaks to the 'git state' alias.
  * Added .gitconfig template with state and last aliases.
  * Added batch file for renaming branches on local and remote git repos. Will eventually be replaced with a git alias.
  * Replaced the Content/char directory with Content/mech.
  * Added material-related naming conventions.
  * Updated the materials library. added: mf_distance_fade (kind of what it sounds like). added: mf_fresnel_gradient (this may not work at the moment). added: mf_fuzzy_shading (more customizable version of Epic's fuzzy shading function). added: mf_highprecision_worldpos_sampling (I hope it's what it sounds like because I don't remember even writing it). added: mf_luminance_adaptive (calculates the luminance of a single buffer sample, optionally taking eye adaptation into account). added: mf_metallic_shading (exaggerated shading for metallic materials; based on Epic's with iterations on our side). added: smooth step material function; changed scalar-based one based on research, float3 one is pretty standard. added: SRGB->Linear color conversation using a best-fit value. added: A whole lot of texture bombing functions in order to split out the complexity from Epic's one-stop-shopping one. Note: still very expensive. added: mf_truesky_directional_light (just a copy of trueSKY's shadow mapping function). added: mf_wind (gets a float4 from a material parameter collection and uses the X/Y as wind direction and W as wind speed). added: the miscellaneous post-effect sharpening materials I've been playing with.
  * Misc. updates and fixed the list views.
  * Updated CONVENTIONS.md with some filename prefixes.
  * Created CONVENTIONS.md and got it kinda started.
  * Fleshing out the README.md with informations!
  * Tried adding Sign Bunny to the greeting. Failed.
  * Adding some UE4 assets that I dig.
  * Adding git attribute/ignore/LFS templates.
  * Updated INSTALL instructions.
  * Updated powershell script installation instructions with a note that I'm an idiot.
  * Adding PowerShell 5 scripts/tools, a sample profile, and setup instructions.
  * Added tool to automate channel-packing for megascan textures.
  * More export artifact clean-up.
  * Fixed some basic export artifacts from Notion.
  * I'm dumb.
  * Fixing up the header.
  * Adding in our internal Todoist task management practices doc.
  * Updated the README with a better header.
  * Initial commit
