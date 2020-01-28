pkgs: attrs:
  with pkgs;
  let defaultAttrs = {
    builder = "${bash}/bin/bash";
    args = [ ./builder.sh ];
    setup = ./setup.sh;
    baseInputs = [ gnumake clang clang-tools binutils-unwrapped coreutils findutils ];
    buildInputs = [];
    system = builtins.currentSystem;
  };
  in
derivation (defaultAttrs // attrs)
