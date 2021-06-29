#include <sys/types.h>
#include <sys/wait.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <signal.h>
#include <syscall.h>
#include <errno.h>
#include <stdlib.h>
#include <seccomp.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
	int rc, fd = -1, status;
	struct seccomp_notif *req = NULL;
	struct seccomp_notif_resp *resp = NULL;
	scmp_filter_ctx ctx = NULL;
	pid_t pid = 0, root;

	root = getpid();
	printf("root: %d\n", root);

	ctx = seccomp_init(SCMP_ACT_ALLOW);
	if (ctx == NULL)
		return ENOMEM;

	rc = seccomp_rule_add(ctx, SCMP_ACT_NOTIFY, SCMP_SYS(getpid), 0, NULL);
	if (rc)
		goto out;

	rc  = seccomp_load(ctx);
	if (rc < 0)
		goto out;

	rc = seccomp_notify_fd(ctx);
	if (rc < 0)
		goto out;
	fd = rc;

	pid = fork();
	if (pid == 0) {
		/* child */
		exit(syscall(__NR_getpid) != root);

	}
	rc = seccomp_notify_alloc(&req, &resp);
	if (rc)
		goto out;

	rc = seccomp_notify_receive(fd, req);
	if (rc)
		goto out;
	if (req->data.nr != __NR_getpid) {
		rc = -EFAULT;
		goto out;
	}
	rc = seccomp_notify_id_valid(fd, req->id);
	if (rc)
		goto out;

	resp->id = req->id;
	resp->val = 100;
	resp->error = 0;
	resp->flags = 0;
	rc = seccomp_notify_respond(fd, resp);
	if (rc)
		goto out;

	if (waitpid(pid, &status, 0) != pid) {
		rc = -EFAULT;
		goto out;
	}

	if (!WIFEXITED(status)) {
		rc = -EFAULT;
		goto out;
	}
	if (WEXITSTATUS(status)) {
		rc = -EFAULT;
		goto out;
	}
	/*
	rc = seccomp_reset(ctx, SCMP_ACT_ALLOW);
	if (rc < 0)
		goto out;

	rc = seccomp_rule_add(ctx, SCMP_ACT_NOTIFY, SCMP_SYS(getppid), 0, NULL);
	if (rc)
		goto out;

	rc  = seccomp_load(ctx);
	if (rc < 0)
		goto out;

	rc = seccomp_notify_fd(ctx);
	if (rc < 0)
		goto out;
	if (rc != fd) {
		rc = -EFAULT;
		goto out;
	} else
		rc = 0;
	*/

out:
	if (fd >= 0)
		close(fd);
	if (pid)
		kill(pid, SIGKILL);
	seccomp_notify_free(req, resp);
	seccomp_release(ctx);

	if (rc != 0)
		return (rc < 0 ? -rc : rc);
	return 160;
}
