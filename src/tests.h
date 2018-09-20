
//# run_tests: Run all tests. Returns true iff they all pass.
//#
//# show_all_tap_results:
//#   If `true`, print valid TAP (Test Anything Protocol) output.
//#   If `false`, only print failures.
//#
bool run_tests(bool show_all_tap_results);

//# tap_ok: Report a test result.
//#
//# success:
//#   `true` if ok, `false` if not ok.
//#
//# test_name:
//#   Name to print by result.
//#
void tap_ok(bool is_ok, const char *test_name);

