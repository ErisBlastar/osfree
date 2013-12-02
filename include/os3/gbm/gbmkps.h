/*

gbmkps.h - IBM KIPS support

*/

extern GBM_ERR kps_qft(GBMFT *gbmft);
extern GBM_ERR kps_rhdr(const char *fn, int fd, GBM *gbm, const char *opt);
extern GBM_ERR kps_rpal(int fd, GBM *gbm, GBMRGB *gbmrgb);
extern GBM_ERR kps_rdata(int fd, GBM *gbm, gbm_u8 *data);
extern GBM_ERR kps_w(const char *fn, int fd, const GBM *gbm, const GBMRGB *gbmrgb, const gbm_u8 *data, const char *opt);
extern const char *kps_err(GBM_ERR rc);
