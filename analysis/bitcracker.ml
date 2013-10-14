open Batteries
open Common
open Cmdliner

let version = "0.0.1"

let source = ()

let help_subcommands = [
  `S "COMMON OPTIONS";
  `P "These options are common to all commands.";
  `S "MORE HELP";
  `P "Use `$(mname) $(i,COMMAND) --help' for help on a single command.";`Noblank;
  `P "Use `$(mname) help patterns' for help on patch matching."; `Noblank;
  `P "Use `$(mname) help environment' for help on environment variables.";
  `S "BUGS"; `P "Check bug reports at http://bugs.example.org.";]

let common_opts debug = { co_debug = debug }

let common_opts_t = 
  let docs = "COMMON OPTIONS" in 
  let debug = 
    let doc = "Give only debug output." in
    Arg.(value & flag & info ["debug"] ~docs ~doc)
  in
  Term.(pure common_opts $ debug)

let default_prompt = 
  let doc = "a bit sequence decoder multi-tool" in 
  let man = help_subcommands in
  Term.(ret (pure (fun _ -> `Help (`Pager, None)) $ common_opts_t)),
  Term.info "bitcracker" ~version ~sdocs:"COMMON OPTIONS" ~doc ~man

let source =
  let doc = "Source data" in  
  Arg.(required & pos 0 (some string) None & info [] ~doc)

let cmd_seqlen = 
  Term.(pure Seqlen.seqlen $ common_opts_t $ source),
  Term.info "seqlen" ~version

let main () = 
  match Term.eval_choice default_prompt [cmd_seqlen] with `Error _ -> exit 1 | _ -> exit 0

let _ = main ()


