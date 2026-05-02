; ModuleID = 'EvaLLVM'
source_filename = "EvaLLVM"

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  ret i32 42
}
