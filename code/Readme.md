# Code

Welcome to the brain section of the project.

## Config

There exists the `config.h` file. Here you can define:
- Number of users and their names
- Max username length
- Sleep time (time between auto-syncs)

The type of things to be tracked are somewhat hardcoded and can't be changed that easily. That's a bit sad, but it can't be helped. This may be changed in the future.

## Structure

- `button` - handles IO events of 1 button
- `display` - everything that is displayed and menu stuff
- `input` - handles IO (single press, double press, long press)
- `main` - inits everything and then loops
- `message` - handles the send and recv of the sync messages
