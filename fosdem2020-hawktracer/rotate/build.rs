fn main() {
    println!(
        "cargo:rustc-link-search=native={}",
        std::env::var("DOWNLOADER_LIB").unwrap()
    );
    println!("cargo:rustc-link-lib=downloader");
}
