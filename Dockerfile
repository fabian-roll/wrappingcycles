FROM ubuntu:jammy AS setup
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update \
&& apt-get install --no-install-recommends --assume-yes --allow-downgrades \
    curl \
    make \
    g++ \
    libgudhi-dev \
    python3.10 \
    python3-pandas \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

RUN pip3 install plyfile

WORKDIR /wrappingcycles

COPY . .

RUN make

RUN mkdir wrappingcycles_output


FROM setup AS computation_bunny

RUN curl -LO "http://graphics.stanford.edu/pub/3Dscanrep/bunny.tar.gz" \
&& tar xf bunny.tar.gz \
&& mv bunny/reconstruction/bun_zipper.ply wrappingcycles_output/ \
&& mv bunny/reconstruction/bun_zipper_res4.ply wrappingcycles_output/ \
&& rm -rf bunny bunny.tar.gz

RUN python3 ply-to-pts.py wrappingcycles_output/bun_zipper.ply /wrappingcycles/bun_zipper_point_cloud.txt \
&& python3 ply-to-pts.py wrappingcycles_output/bun_zipper_res4.ply /wrappingcycles/bun_zipper_res4_point_cloud.txt

RUN python3 persistentfeature-to-ply.py bun_zipper_point_cloud.txt 3 3 wrappingcycles_output/bun_zipper \
&& python3 persistentfeature-to-ply.py bun_zipper_res4_point_cloud.txt 3 3 wrappingcycles_output/bun_zipper_res4

RUN rm bun_zipper_point_cloud.txt bun_zipper_res4_point_cloud.txt


FROM setup AS computation_dragon

RUN curl -LO "http://graphics.stanford.edu/pub/3Dscanrep/dragon/dragon_recon.tar.gz" \
&& tar xf dragon_recon.tar.gz \
&& mv dragon_recon/dragon_vrip.ply wrappingcycles_output/ \
&& rm -rf dragon_recon dragon_recon.tar.gz

RUN python3 ply-to-pts.py wrappingcycles_output/dragon_vrip.ply /wrappingcycles/dragon_vrip_point_cloud.txt 

RUN python3 persistentfeature-to-ply.py dragon_vrip_point_cloud.txt 3 3 wrappingcycles_output/dragon_vrip

RUN rm dragon_vrip_point_cloud.txt


FROM setup AS computation_happy

RUN curl -LO "http://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz" \
&& tar xf happy_recon.tar.gz \
&& mv happy_recon/happy_vrip.ply wrappingcycles_output/ \
&& rm -rf happy_recon happy_recon.tar.gz

RUN python3 ply-to-pts.py wrappingcycles_output/happy_vrip.ply /wrappingcycles/happy_vrip_point_cloud.txt 

RUN python3 persistentfeature-to-ply.py happy_vrip_point_cloud.txt 3 3 wrappingcycles_output/happy_vrip

RUN rm happy_vrip_point_cloud.txt


FROM scratch AS export

COPY --from=computation_bunny /wrappingcycles/wrappingcycles_output /

COPY --from=computation_dragon /wrappingcycles/wrappingcycles_output /

COPY --from=computation_happy /wrappingcycles/wrappingcycles_output /