extern crate rust_hawktracer;
use rust_hawktracer::*;
use std::ffi::CString;
use std::fs::File;
use std::io::Read;
use std::os::raw::c_char;
use std::thread;

use pyo3::prelude::*;

use s3::bucket::Bucket;
use s3::credentials::Credentials;

extern "C" {
    fn download_file(url: *const c_char, destination: *const c_char) -> i32;
}

#[hawktracer(download_file_rs)]
fn download_file_rs(url: &str, destination: &str) -> i32 {
    unsafe {
        download_file(
            CString::new(url).unwrap().as_ptr(),
            CString::new(destination).unwrap().as_ptr(),
        )
    }
}

#[hawktracer(rotate_in_python)]
fn rotate_in_python(image_path: &str, file_pattern: &str) {
    let code = include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/rotate.py"));
    let gil = Python::acquire_gil();
    let executor = PyModule::from_code(gil.python(), &code, "rotate.py", "rotate").unwrap();
    executor.call1("main", (image_path, file_pattern)).unwrap();
}

#[hawktracer(upload_to_s3)]
fn read_a_file(path: &str) -> Vec<u8> {
    let mut file = File::open(path).unwrap();

    let mut data = Vec::new();
    file.read_to_end(&mut data).unwrap();

    return data;
}

#[hawktracer(upload_to_s3)]
fn upload_to_s3(path: &str, name: &str) {
    let mut data = Vec::new();
    let mut file = File::open(path).unwrap();
    file.read_to_end(&mut data).unwrap();

    {
        scoped_tracepoint!(_upload);
        let bucket = Bucket::new(
            "mkolny-fosdem-2020",
            "eu-west-1".parse().unwrap(),
            Credentials::default(),
        )
        .unwrap();
        bucket
            .put_object(name, &data, "application/octet-stream")
            .unwrap();
    }
}

fn main() {
    let instance = HawktracerInstance::new();
    let _listener = instance.create_listener(HawktracerListenerType::ToFile {
        file_path: "rotate.htdump".into(),
        buffer_size: 999999,
    });

    let url = "https://www.hawktracer.org/hawk.jpg";
    let out_file_pattern = "rotated_{}.jpg";

    let tmp_image = "/tmp/fosdem_demo_output.jpg";
    let code = download_file_rs(url, tmp_image);
    println!("Download result: {}", code);

    let h1 = thread::spawn(move || {
        rotate_in_python(tmp_image, &out_file_pattern);
    });

    let h2 = thread::spawn(move || {
        upload_to_s3(tmp_image, "backup.jpg");
    });

    h1.join().unwrap();
    h2.join().unwrap();
}
