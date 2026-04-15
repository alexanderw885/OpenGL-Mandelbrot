## Save pictures
- split image into multiple framebuffers to allow for larger photos

## Dynamic shader creation
- Change from janky regex solution to proper solution using defines in the shader

## Config file
- colormap
- add button to reload application with updated config file
    - reset camera or keep where it is?
- multiple different configs, or just one
    - CLI argument? 
    - runtime selection box?

### Animation configuration file
- Initial parameters
- For each event:
    - parameter to change
    - start time
    - end time
    - function to apply
    
## In-window UI
- Show runtime parameters
- add input boxes and/or sliders?
- let you modify Z/C in real time