/*

gbmjpg.h - JPEG format

*/

#ifdef ENABLE_IJG

extern GBM_ERR jpg_qft(GBMFT *gbmft);
extern GBM_ERR jpg_rhdr(const char *fn, int fd, GBM *gbm, const char *opt);
extern GBM_ERR jpg_rpal(int fd, GBM *gbm, GBMRGB *gbmrgb);
extern GBM_ERR jpg_rdata(int fd, GBM *gbm, gbm_u8 *data);
extern GBM_ERR jpg_w(const char *fn, int fd, const GBM *gbm, const GBMRGB *gbmrgb, const gbm_u8 *data, const char *opt);
extern const char *jpg_err(GBM_ERR rc);

#endif
