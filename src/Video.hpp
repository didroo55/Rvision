class Video {
public:
  Video();
  Video(std::string filename);
  bool open(std::string filename);
  bool isOpened();
  void release();
  Image readNext();
  Image readFrame(int frameId);
  bool set(std::string propId, double value);
  double get(std::string propId);
  Rcpp::NumericVector dim();
  int nrow(), ncol(), nframes();
  double fps();
  std::string codec();

private:
  cv::VideoCapture video;
};

Video::Video() {

}

Video::Video(std::string inputFile) {
  Rcpp::Environment base = Rcpp::Environment::base_env();
  Rcpp::Function pathExpand = base["path.expand"];

  this->video.open(Rcpp::as<std::string>(pathExpand(inputFile)));

  if (!this->video.isOpened()) {
    throw std::range_error("Could not open the video.");
  }
}

bool Video::open(std::string inputFile) {
  Rcpp::Environment base = Rcpp::Environment::base_env();
  Rcpp::Function pathExpand = base["path.expand"];

  this->video.open(Rcpp::as<std::string>(pathExpand(inputFile)));

  if (!this->video.isOpened()) {
    throw std::range_error("Could not open the video.");
  } else {
    return true;
  }
}

bool Video::isOpened() {
  return this->video.isOpened();
}

void Video::release() {
  this->video.release();
}

bool Video::set(std::string propId, double value) {
  return this->video.set(getPropId(propId), value);
}

double Video::get(std::string propId) {
  return this->video.get(getPropId(propId));
}

Rcpp::NumericVector Video::dim() {
  return Rcpp::NumericVector::create(this->video.get(cv::CAP_PROP_FRAME_HEIGHT),
                                     this->video.get(cv::CAP_PROP_FRAME_WIDTH),
                                     this->video.get(cv::CAP_PROP_FRAME_COUNT));
}

int Video::nrow() {
  return this->video.get(cv::CAP_PROP_FRAME_HEIGHT);
}

int Video::ncol() {
  return this->video.get(cv::CAP_PROP_FRAME_WIDTH);
}

int Video::nframes() {
  return this->video.get(cv::CAP_PROP_FRAME_COUNT);
}

double Video::fps() {
  return this->video.get(cv::CAP_PROP_FPS);
}

std::string Video::codec() {
  union {
    char    c[5];
    int     i;
  } fourcc;
  fourcc.i = this->video.get(CV_CAP_PROP_FOURCC);
  fourcc.c[4] = '\0';
  return fourcc.c;
}

Image Video::readNext() {
  cv::Mat outputFrame;
  this->video.read(outputFrame);

  return Image(outputFrame);
}

Image Video::readFrame(int frameId) {
  if (frameId > this->video.get(CV_CAP_PROP_FRAME_COUNT)) {
    throw std::range_error("The requested frame does not exist. Try with a lower frame number.");
  }

  cv::Mat outputFrame;

  this->video.set(CV_CAP_PROP_POS_FRAMES, frameId);
  this->video.read(outputFrame);

  return Image(outputFrame);
}



