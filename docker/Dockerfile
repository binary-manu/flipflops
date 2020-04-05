FROM archlinux/base
RUN pacman-key --init && pacman-key --populate archlinux && \
    pacman -Syu --noconfirm dosbox && \
    rm -f /var/lib/pacman/sync/* /var/cache/pacman/pkg/*

WORKDIR /build
ENV SDL_VIDEODRIVER=dummy \
    SDL_AUDIODRIVER=dummy
ENTRYPOINT [ "/entry.sh" ]

COPY entry.sh /
