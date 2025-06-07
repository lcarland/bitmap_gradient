This is just a simple bit of code designed to generate a gradient bitmap image.

bmpgen generates a bitmap gradient depending on the values set in image.conf.\
If image.conf is not present, you will need to add the **config** argument to generate the file. You can also use this to reset the config file to defaults.
```
bin/bmpgen config
```
>*I don't have any config validations implemented yet, so typos or bad syntax will likely cause the program to crash*

Run *bmpgen* without arguments to generate an image from parameters defined in image.conf.
```
bin/bmpgen
```
Parameters in the config file can be overriden with arguments using a *key=value* syntax
```
bin/bmpgen redStart=200 gradient_direction=horizontal
```

