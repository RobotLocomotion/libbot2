n.b. for running `//scripts/package/linux/ubuntu/noble/package`:

The `package` script must be run from within the `libbot2` repository, either
at the root or within a subdirectory. Note that the script will attempt to
write the packages in the directory where the script was invoked. For the sake
of simplicity and consistency, it may be beneficial to run the script
exclusively at the root of the repo.

Regardless of where the script is run, in order for the write to succeed, the
dispatching directory needs to have global write permissions enabled. If the
script is run from `//libbot2/foo/`, then the following must be run as a
prerequisite to the `package` script:

  ```
  $ sudo chmod a+w </path/to/libbot2/foo>
  $ source ~/.bashrc
  ```

(Note that if you use a different shell or configuration file, you will have to
change the path provided for the second command.)
