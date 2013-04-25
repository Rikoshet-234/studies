require 'redcarpet/compat'

class AuctionsController < ApplicationController
  before_filter :signed_in, only: [:new, :create, :edit, :update, :finish]
  before_filter :correct_user, only: [:edit, :update, :finish]

  def index
    if params[:tag]
      @tag = Tag.find_by_name!(params[:tag])
      @auctions = @tag.auctions(params[:tag])
    else
      @auctions = Auction.all
    end
  end

  def show
    @auction = Auction.find(params[:id])
    @content = Markdown.new(@auction.content).to_html

    if signed_in?
      @bid = Bid.new
      @bid.user = current_user
      @bid.auction = @auction

      if @auction.bids.first
        @bid_value = (@auction.bids.first.value * 120.0).ceil / 100.0
      else
        @bid_value = 1.00
      end
    end
  end

  def new
    @auction = current_user.auctions.build
  end

  def create
    @auction = current_user.auctions.build(params[:auction])
    if @auction.save
      flash[:success] = 'Auction created successfully.'
      redirect_to @auction
    else
      flash[:error] = "#{@auction.errors.count} errors, cannot proceed."
      render 'new'
    end
  end

  def edit
  end

  def update
    if @auction.update_attributes(params[:auction])
      flash[:success] = 'All changes saved successfully.'
      redirect_to @auction
    else
      flash[:error] = "#{@auction.errors.count} errors, cannot save changes."
      render 'edit'
    end
  end

  def finish
    @auction = Auction.find(params[:id])
    if @auction.update_attributes(finish_date: Time.now)
      flash[:success] = 'Auction has been finished.'
    else
      flash[:error] = 'Something went wrong...'
    end
    redirect_to @auction
  end

  private

  def signed_in
    unless signed_in?
      flash[:notice] = 'Only signed in users have access to this site.'
      redirect_to signin_path
    end
  end

  def correct_user
    @auction = Auction.find(params[:id])
    unless current_user?(@auction.user)
      flash[:notice] = 'You are not allowed to view this page.'
      redirect_to root_path
    end
  end

end
