#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>

struct pam_response *reply;

int null_conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
	*resp = reply;
	return PAM_SUCCESS;
}

static struct pam_conv conv = {null_conv, NULL};

int main(int argc, char *argv[])
{
	int ret = 1;
	const char *user, *pass, *pam_service = "common-auth";
	pam_handle_t *pamh = NULL;

	if (argc < 3)
		return 3;
	user = argv[1];
	pass = argv[2];
	if (argc > 3)
		pam_service = argv[3];

	if (pam_start(pam_service, user, &conv, &pamh) == PAM_SUCCESS)
	{
		reply = (struct pam_response *)malloc(sizeof(struct pam_response));
		reply->resp = strdup(pass);
		reply->resp_retcode = 0;
		
		ret = pam_authenticate(pamh, 0);
		switch (ret)
		{
			case PAM_SUCCESS:
				ret = 0;
				break;
			case PAM_USER_UNKNOWN:
				ret = 2;
				break;
			default:
				printf("%s\n", pam_strerror(pamh, ret));
				ret = 1;
				break;
		}
		pam_end(pamh, PAM_SUCCESS);
	}
	return ret;
}
